<?php

namespace AppBundle\Entity;

use Doctrine\ORM\Mapping as ORM;

/**
 * @ORM\Entity
 * @ORM\Table(name="patch")
 * @IgnoreAnnotation("var", "return", "param")
 */
class Patch
{
    /**
     * @ORM\Column(name="event_name", type="string", length=50)
     * @ORM\Id
     * @ORM\GeneratedValues(strategy="AUTO")
     *
     * @var string
     */
    private $eventName;

    /**
     * @ORM\Column(type="integer", nullable=false)
     * @ORM\Id
     * @ORM\GeneratedValues(strategy="AUTO")
     *
     * @var integer
     */
    private $dmx;

    /**
     * @ORM\Column(type="integer")
     *
     * @var integer
     */
    private $channel;

    /**
     * @ORM\Column(type="integer")
     *
     * @var integer
     */
    private $intensity;

    /**
     * @ORM\Column(type="string", length=3)
     *
     * @var string
     */
    private $type;


    /**
     * Get eventName
     *
     * @return string 
     */
    public function getEventName()
    {
        return $this->eventName;
    }

    /**
     * Get dmx
     *
     * @return integer 
     */
    public function getDmx()
    {
        return $this->dmx;
    }

    /**
     * Set channel
     *
     * @param integer $channel
     * @return Patch
     */
    public function setChannel($channel)
    {
        $this->channel = $channel;

        return $this;
    }

    /**
     * Get channel
     *
     * @return integer 
     */
    public function getChannel()
    {
        return $this->channel;
    }

    /**
     * Set intensity
     *
     * @param integer $intensity
     * @return Patch
     */
    public function setIntensity($intensity)
    {
        $this->intensity = $intensity;

        return $this;
    }

    /**
     * Get intensity
     *
     * @return integer 
     */
    public function getIntensity()
    {
        return $this->intensity;
    }

    /**
     * Set type
     *
     * @param string $type
     * @return Patch
     */
    public function setType($type)
    {
        $this->type = $type;

        return $this;
    }

    /**
     * Get type
     *
     * @return string 
     */
    public function getType()
    {
        return $this->type;
    }

}
