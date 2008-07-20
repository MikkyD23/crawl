/*
 *  File:       beam.h
 *  Summary:    Functions related to ranged attacks.
 *  Written by: Linley Henzell
 *
 *  Modified for Crawl Reference by $Author$ on $Date$
 *
 *  Change History (most recent first):
 *
 *               <1>     -/--/--        LRH             Created
 */


#ifndef BEAM_H
#define BEAM_H


#include "externs.h"
#include "ray.h"

class monsters;

enum demon_beam_type
{
    DMNBM_HELLFIRE,                    //    0
    DMNBM_SMITING,
    DMNBM_BRAIN_FEED,
    DMNBM_MUTATION
};

// must match wand subtypes! (see item_def::zap())
enum zap_type
{
    ZAP_FLAME,                         //    0
    ZAP_FROST,
    ZAP_SLOWING,
    ZAP_HASTING,
    ZAP_MAGIC_DARTS,
    ZAP_HEALING,                       //    5
    ZAP_PARALYSIS,
    ZAP_FIRE,
    ZAP_COLD,
    ZAP_CONFUSION,
    ZAP_INVISIBILITY,                  //   10
    ZAP_DIGGING,
    ZAP_FIREBALL,
    ZAP_TELEPORTATION,
    ZAP_LIGHTNING,
    ZAP_POLYMORPH_OTHER,               //   15
    ZAP_VENOM_BOLT,
    ZAP_NEGATIVE_ENERGY,
    ZAP_CRYSTAL_SPEAR,
    ZAP_BEAM_OF_ENERGY,
    ZAP_MYSTIC_BLAST,                  //   20
    ZAP_ENSLAVEMENT,
    ZAP_PAIN,
    ZAP_STICKY_FLAME,
    ZAP_DISPEL_UNDEAD,
    ZAP_CLEANSING_FLAME,               //   25
    ZAP_BONE_SHARDS,
    ZAP_BANISHMENT,
    ZAP_DEGENERATION,
    ZAP_STING,
    ZAP_HELLFIRE,                      //   30
    ZAP_IRON_BOLT,
    ZAP_STRIKING,
    ZAP_STONE_ARROW,
    ZAP_ELECTRICITY,
    ZAP_ORB_OF_ELECTRICITY,            //   35
    ZAP_SPIT_POISON,
    ZAP_DEBUGGING_RAY,
    ZAP_BREATHE_FIRE,
    ZAP_BREATHE_FROST,
    ZAP_BREATHE_ACID,                  //   40
    ZAP_BREATHE_POISON,
    ZAP_BREATHE_POWER,
    ZAP_ENSLAVE_UNDEAD,
    ZAP_AGONY,
    ZAP_DISRUPTION,                    //   45
    ZAP_DISINTEGRATION,                //   46
    // ZAP_ISKS_CROSS, //   47: Isk's Cross -- commented out, deprecated {dlb}
    ZAP_BREATHE_STEAM = 48,            //   48
    ZAP_CONTROL_DEMON,
    ZAP_ORB_OF_FRAGMENTATION,          //   50
    ZAP_ICE_BOLT,
    ZAP_ICE_STORM,
    ZAP_BACKLIGHT,                     //jmf: added next bunch 19mar2000
    ZAP_SLEEP,
    ZAP_FLAME_TONGUE,
    ZAP_SANDBLAST,
    ZAP_SMALL_SANDBLAST,
    ZAP_MAGMA,
    ZAP_POISON_ARROW,
    ZAP_BREATHE_STICKY_FLAME,
    ZAP_BREATHE_LIGHTNING,
    ZAP_PETRIFY,
    ZAP_HELLFROST,
    NUM_ZAPS                           // must remain last member {dlb}
};

struct dist;

struct bolt
{
    // INPUT parameters set by caller
    int         range;                 // minimum range
    int         rangeMax;              // maximum range
    unsigned    type;                  // missile gfx
    int         colour;
    beam_type   flavour;
    int         source_x, source_y;    // beam origin
    dice_def    damage;
    int         ench_power, hit;
    int         target_x, target_y;    // intended target
    coord_def   pos;                   // actual position
    killer_type thrower;               // what kind of thing threw this?
    char        ex_size;               // explosion radius (0==none)
    int         beam_source;           // NON_MONSTER or monster index #
    std::string name;
    bool        is_beam;               // beams? (can hits multiple targets?)
    bool        is_explosion;
    bool        is_big_cloud;          // expands into big_cloud at endpoint
    bool        is_enchant;            // no block/dodge, but mag resist
    bool        is_energy;             // mostly energy/non-physical attack
    bool        is_launched;           // was fired from launcher?
    bool        is_thrown;             // was thrown from hand?
    bool        target_first;          // targeting by direction
    bool        aimed_at_spot;         // aimed at (x,y), should not cross
    std::string aux_source;            // source of KILL_MISC beams

    bool        affects_nothing;       // should not hit monsters or features

    // OUTPUT parameters (tracing, ID)
    bool        obvious_effect;        // did an 'obvious' effect happen?
    bool        effect_known;          // did we _know_ this would happen?
    int         fr_count, foe_count;   // # of times a friend/foe is "hit"
    int         fr_power, foe_power;   // total levels/hit dice affected
    int         fr_hurt, foe_hurt;     // # of friends/foes actually hurt
    int         fr_helped, foe_helped; // # of friends/foes actually helped

    // INTERNAL use - should not usually be set outside of beam.cc
    bool        is_tracer;       // is this a tracer?
    bool        aimed_at_feet;   // this was aimed at self!
    bool        msg_generated;   // an appropriate msg was already mpr'd
    bool        in_explosion_phase;   // explosion phase (as opposed to beam phase)
    bool        smart_monster;   // tracer firer can guess at other mons. resists?
    bool        can_see_invis;   // tracer firer can see invisible?
    mon_attitude_type attitude;  // attitude of whoever fired tracer
    int         foe_ratio;       // 100* foe ratio (see mons_should_fire())
    bool        chose_ray;       // do we want a specific ray?
    bool        beam_cancelled;  // stop_attack_prompt() returned true
    bool        dont_stop_foe;   // stop_attack_prompt() returned false for foe
    bool        dont_stop_fr;    // stop_attack_prompt() returned false for
                                 // friend
    bool        dont_stop_player; // player answered self target prompt with 'y'

    ray_def     ray;             // shoot on this specific ray

public:
    // A constructor to try and fix some of the bugs that occur because
    // this struct never seems to be properly initialized.  Definition
    // is over in beam.cc.
    bolt();

    void set_target(const dist &);
    void setup_retrace();

    // Returns YOU_KILL or MON_KILL, depending on the source of the beam.
    killer_type  killer() const;

    coord_def target() const
    {
        return (coord_def(target_x, target_y));
    }
};

dice_def calc_dice( int num_dice, int max_damage );

// Test if the to-hit (attack) beats evasion (defence).
bool test_beam_hit(int attack, int defence);

/* ***********************************************************************
 * called from: bang - it_use2 - monstuff - mstuff2
 * *********************************************************************** */
void fire_beam(bolt &pbolt, item_def *item = NULL, bool drop_item = false);

// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: ability - it_use3 - item_use - mstuff2 - religion -
 *              spells - spells4
 * *********************************************************************** */
int explosion( bolt &pbolt, bool hole_in_the_middle = false,
               bool explode_in_wall = false,
               bool stop_at_statues = true,
               bool stop_at_walls   = true,
               bool show_more       = true);

// last updated 22jan2001 {gdl}
/* ***********************************************************************
 * called from: effects - spells2 - spells4
 * *********************************************************************** */
int mons_adjust_flavoured(monsters *monster, bolt &pbolt, int hurted,
                          bool doFlavouredEffects = true);


/* ***********************************************************************
 * called from: ability - item_use - spell
 * returns true if messages were generated during the enchantment
 * *********************************************************************** */
bool mass_enchantment( enchant_type wh_enchant, int pow, int who,
                       int *m_succumbed = NULL, int *m_attempted = NULL );


/* ***********************************************************************
 * called from: fight - monstuff - mstuff2
 * *********************************************************************** */
int mons_ench_f2( monsters *monster, struct bolt &pbolt );


/* ***********************************************************************
 * called from: fight - monstuff - spells2
 * *********************************************************************** */
bool poison_monster( monsters *monster, kill_category who,
                     int levels = 1, bool force = false, bool verbose = true );


/* ***********************************************************************
 * called from: monstuff
 * *********************************************************************** */
void fire_tracer( const monsters *monster, struct bolt &pbolt,
                  bool explode_only = false );

bool check_line_of_sight( int sx, int sy, int tx, int ty );

/* ***********************************************************************
 * called from: monstuff
 * *********************************************************************** */
void mimic_alert( monsters *mimic );


bool zapping( zap_type ztype, int power, struct bolt &pbolt,
              bool needs_tracer = false, std::string msg = "" );

bool player_tracer( zap_type ztype, int power, struct bolt &pbolt,
                    int range = 0 );

int affect(bolt &beam, int x, int y, item_def *item = NULL);

void beam_drop_object( bolt &beam, item_def *item, int x, int y );

#endif
